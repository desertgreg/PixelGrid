
namespace PGImageTool
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.SelectImagesButton = new System.Windows.Forms.Button();
            this.SrcFilesListView = new System.Windows.Forms.ListView();
            this.SelectOutputButton = new System.Windows.Forms.Button();
            this.OutputFileTextBox = new System.Windows.Forms.TextBox();
            this.GenerateButton = new System.Windows.Forms.Button();
            this.openFileDialog = new System.Windows.Forms.OpenFileDialog();
            this.saveFileDialog = new System.Windows.Forms.SaveFileDialog();
            this.SuspendLayout();
            // 
            // SelectImagesButton
            // 
            this.SelectImagesButton.Location = new System.Drawing.Point(17, 16);
            this.SelectImagesButton.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.SelectImagesButton.Name = "SelectImagesButton";
            this.SelectImagesButton.Size = new System.Drawing.Size(265, 28);
            this.SelectImagesButton.TabIndex = 0;
            this.SelectImagesButton.Text = "Select Images";
            this.SelectImagesButton.UseVisualStyleBackColor = true;
            this.SelectImagesButton.Click += new System.EventHandler(this.SelectImagesButton_Click);
            // 
            // SrcFilesListView
            // 
            this.SrcFilesListView.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.SrcFilesListView.GridLines = true;
            this.SrcFilesListView.HideSelection = false;
            this.SrcFilesListView.Location = new System.Drawing.Point(17, 53);
            this.SrcFilesListView.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.SrcFilesListView.Name = "SrcFilesListView";
            this.SrcFilesListView.Size = new System.Drawing.Size(719, 378);
            this.SrcFilesListView.TabIndex = 1;
            this.SrcFilesListView.UseCompatibleStateImageBehavior = false;
            this.SrcFilesListView.View = System.Windows.Forms.View.List;
            // 
            // SelectOutputButton
            // 
            this.SelectOutputButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.SelectOutputButton.Location = new System.Drawing.Point(17, 441);
            this.SelectOutputButton.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.SelectOutputButton.Name = "SelectOutputButton";
            this.SelectOutputButton.Size = new System.Drawing.Size(265, 28);
            this.SelectOutputButton.TabIndex = 2;
            this.SelectOutputButton.Text = "Select Output File";
            this.SelectOutputButton.UseVisualStyleBackColor = true;
            this.SelectOutputButton.Click += new System.EventHandler(this.SelectOutputButton_Click);
            // 
            // OutputFileTextBox
            // 
            this.OutputFileTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.OutputFileTextBox.Location = new System.Drawing.Point(17, 478);
            this.OutputFileTextBox.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.OutputFileTextBox.Name = "OutputFileTextBox";
            this.OutputFileTextBox.Size = new System.Drawing.Size(719, 22);
            this.OutputFileTextBox.TabIndex = 3;
            // 
            // GenerateButton
            // 
            this.GenerateButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.GenerateButton.Location = new System.Drawing.Point(525, 511);
            this.GenerateButton.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.GenerateButton.Name = "GenerateButton";
            this.GenerateButton.Size = new System.Drawing.Size(211, 28);
            this.GenerateButton.TabIndex = 4;
            this.GenerateButton.Text = "Generate";
            this.GenerateButton.UseVisualStyleBackColor = true;
            this.GenerateButton.Click += new System.EventHandler(this.GenerateButton_Click);
            // 
            // openFileDialog
            // 
            this.openFileDialog.FileName = "openFileDialog";
            this.openFileDialog.Filter = "PNG Files | *.png";
            this.openFileDialog.Multiselect = true;
            // 
            // saveFileDialog
            // 
            this.saveFileDialog.DefaultExt = "cpp";
            this.saveFileDialog.Filter = "CPP Files | *.cpp";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(756, 554);
            this.Controls.Add(this.GenerateButton);
            this.Controls.Add(this.OutputFileTextBox);
            this.Controls.Add(this.SelectOutputButton);
            this.Controls.Add(this.SrcFilesListView);
            this.Controls.Add(this.SelectImagesButton);
            this.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.Name = "Form1";
            this.Text = "PGImageTool";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button SelectImagesButton;
        private System.Windows.Forms.ListView SrcFilesListView;
        private System.Windows.Forms.Button SelectOutputButton;
        private System.Windows.Forms.TextBox OutputFileTextBox;
        private System.Windows.Forms.Button GenerateButton;
        private System.Windows.Forms.OpenFileDialog openFileDialog;
        private System.Windows.Forms.SaveFileDialog saveFileDialog;
    }
}

